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

|order| indri query |
|:-----:| ----------- |
| 0 | #combine( colorless green ideas sleep furiously ) | 
| 1 | #weight( 0.85 #combine( Colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( green idea ) #1( idea sleep ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( green idea ) #uw8( idea sleep ) #uw8( sleep furiously ) ) ) | 
| 2 | #weight( 0.85 #combine( Colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( colorless idea ) #1( green idea ) #1( green sleep ) #1( idea sleep ) #1( idea furiously ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( colorless idea ) #uw8( green idea ) #uw8( green sleep ) #uw8( idea sleep ) #uw8( idea furiously ) #uw8( sleep furiously ) ) ) |
| -1 | #weight( 0.85 #combine( Colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( colorless idea ) #1( colorless sleep ) #1( colorless furiously ) #1( green idea ) #1( green sleep ) #1( green furiously ) #1( idea sleep ) #1( idea furiously ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( colorless idea ) #uw8( colorless sleep ) #uw8( colorless furiously ) #uw8( green idea ) #uw8( green sleep ) #uw8( green furiously ) #uw8( idea sleep ) #uw8( idea furiously ) #uw8( sleep furiously ) ) ) |

This implementation also includes condensed list dependence models which, like condensed list relevance models, first conduct a query likelihood retrieval and then rerank the results using the dependence model.  This can substantially improve speed, especially for longer queries.  

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
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

