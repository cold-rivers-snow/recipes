from boto3.session import Session
import boto3
import urllib
import base64
import hashlib
import logging
logging.basicConfig(level=logging.DEBUG)
import os

# 设置全局代理（脚本运行期间生效）
# os.environ["HTTP_PROXY"] = "http://127.0.0.1:7897"
# os.environ["HTTPS_PROXY"] = "http://代理IP:端口"

ak = "123"
sk = "123"
url="http://s3.amazonaws.com/"

session = Session(ak, sk, region_name='default')
client = session.client("s3", endpoint_url=url, config=boto3.session.Config(signature_version='s3',connect_timeout=3000000, read_timeout=3000000))
r = client.put_object(
    Bucket='bk1',
    Key='1',
    Body='a'*1024,
    StorageClass='GLACIER'
)
print(r)
