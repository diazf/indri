indri
=====

This is a clone of Indri 5.12 with minor customizations.  

## customizations
### Condensed List Relevance Models

Added a few lines of code to `IndriRunQuery` to support  [Condensed List Relevance Models](https://dl.acm.org/citation.cfm?id=2808194.2809491).  When executing `IndriRunQuery`, just include `-rerankSize=k` to have the RM rerank `k` from an initial query likelihood retrieval.

### External Expansion

Added a few lines of code to `IndriRunQuery` to support  [External Expansion](https://dl.acm.org/citation.cfm?doid=1148170.1148200).  When executing `IndriRunQuery`, just include `-externalIndex=<PATH>` to have the RM built from an initial query likelihood retrieval on the external index.

### Dependence Models

Added order-`k` [dependence models](https://doi.org/10.1145/1076034.1076115).  Order `k=1` dependence models are the classic sequential dependence model.  Order `k` computes the dependences between all query terms within a `k` word window.  Order `k=-1` is the classic full dependence model.  

This implementation also includes condensed list dependence models which, like condensed list relevance models, first conduct a query likelihood retrieval and then rerank the results using the dependence model.  This can substantially improve speed, especially for longer queries.  

| parameter | type | default | description |
| --------- | ---- | ------- | ----------- |
| order | int | 1 | dependence model order |
| combineWeight | float | 0.85 | weight for combine subquery |
| owWeight | float | 0.10 | weight for ordered window subquery |
| uwWeight | float | 0.05 | weight for unordered window subquery |
| uwSize | int | 8 | unordered window size |
| rerankSize | int | 0 | number of query likelihood results to rerank (0=retrieve) |

Defaults are used only if order is greater than 0.

For example, to run a classic SDM reranking of a length 100 query likelihood initial retrieval,
```
IndriRunQuery -index=/path/to/index -query="hello world" -dm=order:1,rerank:100
```

Dependence models are built after internally stopping and stemming the query terms.



## Notes

To build on OSX, be should to set `CXXFLAGS=-fno-tree-vectorize`.  Thanks to [Luke Gallagher](https://github.com/lgrz).  

