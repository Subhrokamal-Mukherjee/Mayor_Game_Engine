from sentence_transformers import SentenceTransformer
model = SentenceTransformer("all-MiniLM-L6-v2")

sentences = [ 
    "1. build road"
    "2. build hospital"
]  # from LLM
embeddings = model.encode(sentences)
print(embeddings)