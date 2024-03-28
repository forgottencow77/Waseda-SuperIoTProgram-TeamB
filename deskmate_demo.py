from langchain.tools.base import BaseTool
import os
from pydantic import Field
from typing import Type
from pydantic import BaseModel, Field
from langchain.tools import BaseTool
from langchain_community.document_loaders import TextLoader
from langchain.embeddings.openai import OpenAIEmbeddings
from langchain.text_splitter import CharacterTextSplitter
from langchain_community.vectorstores import Chroma
from langchain.chains import LLMMathChain 
from langchain.agents import Tool
from langchain_openai.chat_models import ChatOpenAI
from langchain_community.utilities import SerpAPIWrapper
from langchain.agents.initialize import initialize_agent, AgentType
from langchain.memory import ConversationBufferMemory
from langchain.prompts import MessagesPlaceholder
from gpiozero import AngularServo
import time

############################################################
#            1 LangchainのAgentをここで準備する               #
############################################################

os.environ["MY_FILE_PATH"] = "/Users/forgottencow/Desktop/Codes_Python/テンカウ技研/Tenkau-Institute-Technology/L.U.N.A - A.I/ChatGPT/myfile.txt"
#FASTCHATサーバーの互換キー🔑
os.environ["OPENAI_API_KEY"] = "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"
#os.environ["OPENAI_API_BASE"] = "http://192.168.1.19:8000/v1"
#os.environ["OPENAI_API_BASE"] = "http://119.105.109.59:8000/v1"

llm = ChatOpenAI(temperature=0, model="gpt-3.5-turbo")
llm_math_chain = LLMMathChain.from_llm(llm=llm, verbose=True)

search = SerpAPIWrapper(
    serpapi_api_key='85a67cb8f59594f69249cd8881727a04548bce71164f6d5b00141f1ef49c2b3f')

############################################################
#             2 Toolsをここで定義する                         #
############################################################


##############txtファイルに会話の要約が保存されるツール###################

class TextFileWriterTool(BaseTool):
    """Tool that writes input to a text file."""

    name = "TextFileWriterTool"
    description = (
        "A tool that writes input to a text file. "
        "Input should be a summarized text of no more than 1000 characters."
    )
    
    # Declare file_path as a class field
    file_path: str = Field(default="")

    def __init__(self) -> None:
        super().__init__()  # Call the parent constructor
        self.file_path = os.getenv('MY_FILE_PATH')
        if self.file_path is None:
            raise ValueError("Environment variable 'MY_FILE_PATH' is not set.")
        return

    def _run(self, tool_input: str) -> str:
        """Use the TextFileWriterTool."""
        # Check if input is less than or equal to 1000 characters
        if len(tool_input) > 1000:
            return "Error: Input text is longer than 500 characters."

        # Append the input to the text file
        with open(self.file_path, 'a') as f:
            f.write(tool_input + '\n')

        return "Text successfully appended to the file."

    async def _arun(self, tool_input: str) -> str:
        """Use the TextFileWriterTool asynchronously."""
        return self._run(tool_input)


##############書き込んだtxtファイルをRAGで検索するツール###################

class QueryArgs(BaseModel):
    query: str = Field(..., description="Search query to be executed.")
    # path: str = Field(..., description="Path to the text document.")

class SimilaritySearchTool(BaseTool):
    name = "similarity_search_tool"
    description = "This tool loads a document, splits it into chunks, embeds each chunk and loads it into the vector store, and finally executes a similarity search."
    args_schema: Type[BaseModel] = QueryArgs  # Updated

    def _load_documents(self, path):
        return TextLoader(path).load()

    def _split_documents(self, raw_documents):
        text_splitter = CharacterTextSplitter(chunk_size=1000, chunk_overlap=0)
        return text_splitter.split_documents(raw_documents)

    def _create_vectorstore(self, documents):
        return Chroma.from_documents(documents, OpenAIEmbeddings())

    def _run(self, query):
        path = os.getenv('MY_FILE_PATH')
        raw_documents = self._load_documents(path)
        documents = self._split_documents(raw_documents)
        db = self._create_vectorstore(documents)
        return db.similarity_search(query)
        
    async def _arun(self, query):  # Async version of _run
        return self._run(query)


##############サーボモーターを操作してくれるツール###################

class ServoMotorControlArgs(BaseModel):
    min_angle: int = Field(0, description="Minimum angle for the servo motor sweep.")
    max_angle: int = Field(180, description="Maximum angle for the servo motor sweep.")
    delay: float = Field(0.001, description="Delay between angle adjustments.")
    
class ServoMotorControlTool(BaseTool):
    name = "servo_motor_control_tool"
    description = "This tool controls a servo motor."
    args_schema: Type[BaseModel] = ServoMotorControlArgs

    def __init__(self) -> None:
        super().__init__()  # Call the parent constructor
        myGPIO=18
        myCorrection=0.0
        maxPW=(2.5+myCorrection)/1000
        minPW=(0.5-myCorrection)/1000
        self.servo =  AngularServo(myGPIO,initial_angle=0,min_angle=0, max_angle=180,min_pulse_width=minPW,max_pulse_width=maxPW)

    def _run(self, min_angle: int, max_angle: int, delay: float) -> str:
        """Use the ServoMotorControlTool."""
        for angle in range(min_angle, max_angle + 1):   # make servo rotate from min_angle to max_angle deg
            self.servo.angle = angle
            time.sleep(delay)
        time.sleep(0.5)
        for angle in range(max_angle, min_angle - 1, -1): # make servo rotate from max_angle to min_angle deg
            self.servo.angle = angle
            time.sleep(delay)
        time.sleep(0.5)
        return f"Servo motor sweep completed from {min_angle} to {max_angle} degrees."

    async def _arun(self, min_angle: int, max_angle: int, delay: float) -> str:
        """Use the ServoMotorControlTool asynchronously."""
        return self._run(min_angle, max_angle, delay)


#####################################################################
#                3  作成したツールをここでリストに代入する                 #
######################################################################
tools = [
    Tool(
        name = "Search",
        func=search.run,
        description="useful for when you need to answer questions about current events. You should ask targeted questions"
    ),
    TextFileWriterTool(),
    SimilaritySearchTool(),
    ServoMotorControlTool()
]


#####################################################################
#                    4   エージェントを作成する                         #
#####################################################################
agent_kwargs = {
    "extra_prompt_messages": [MessagesPlaceholder(variable_name="memory")],
}
memory = ConversationBufferMemory(memory_key="memory", return_messages=True)

#############################エージェントを初期化する#####################

mrkl = initialize_agent(tools, llm, agent=AgentType.OPENAI_FUNCTIONS, agent_kwargs=agent_kwargs, memory=memory, verbose=True)



#####################################################################
#                          5    --会話開始--                          #
######################################################################
user = ""
while user != "exit":
    user = input("入力してください:")
    print(user)
    ai = mrkl.invoke(input=user)
    print(ai)