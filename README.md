indri
=====

This is a clone of Indri 5.12 with minor customizations.  

## IndriRunQuery customizations

Unless noted, all parameters are thrown on the command line just like other IndriRunQuery parameters. 

### [Condensed List Relevance Models](https://dl.acm.org/citation.cfm?id=2808194.2809491)

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| rerankSize | int | 0 | relevance model reranks a length `rerankSize` initial query likelihood retrieval.  |

Using condensed list relevance models can substantially improve speed without degrading effectiveness of relevance models.  This means you can run massive query expansion very quickly.

### [External Expansion](https://dl.acm.org/citation.cfm?doid=1148170.1148200)

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| externalIndex | path | NONE | RM is built from an initial query likelihood retrieval from externalIndex.  |

Using external expansion with a large external index can substantially improve effectiveness of query expansion.  This can be combined with condensed list relevance models (from the target corpus) if you are concerned about speed.  

### [Dependence Models](https://doi.org/10.1145/1076034.1076115)

To construct and run a dependence model query, just use `-dm=<dm parameter:dm parameter value>[,<dm parameter:dm parameter value>]+`.  Defaults are used if any DM parameters are set.

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| order | int | 1 | dependence model order |
| combineWeight | float | 0.85 | weight for combine subquery |
| owWeight | float | 0.10 | weight for ordered window subquery |
| uwWeight | float | 0.05 | weight for unordered window subquery |
| uwSize | int | 8 | unordered window size |
| rerankSize | int | 0 | number of query likelihood results to rerank (0=retrieve) |

Order `k=1` dependence models are the classic sequential dependence model.  Order `k` computes the dependences between all query terms within a `k` word window.  Order `k=-1` is the classic full dependence model.  



|order| indri query |
|:-----:| ----------- |
| 0 | #combine( colorless green ideas sleep furiously ) | 
| 1 | #weight( 0.85 #combine( colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( green idea ) #1( idea sleep ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( green idea ) #uw8( idea sleep ) #uw8( sleep furiously ) ) ) | 
| 2 | #weight( 0.85 #combine( colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( colorless idea ) #1( green idea ) #1( green sleep ) #1( idea sleep ) #1( idea furiously ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( colorless idea ) #uw8( green idea ) #uw8( green sleep ) #uw8( idea sleep ) #uw8( idea furiously ) #uw8( sleep furiously ) ) ) |
| -1 | #weight( 0.85 #combine( colorless green ideas sleep furiously ) 0.1 #combine( #1( colorless green ) #1( colorless idea ) #1( colorless sleep ) #1( colorless furiously ) #1( green idea ) #1( green sleep ) #1( green furiously ) #1( idea sleep ) #1( idea furiously ) #1( sleep furiously ) ) 0.05 #combine( #uw8( colorless green) #uw8( colorless idea ) #uw8( colorless sleep ) #uw8( colorless furiously ) #uw8( green idea ) #uw8( green sleep ) #uw8( green furiously ) #uw8( idea sleep ) #uw8( idea furiously ) #uw8( sleep furiously ) ) ) |

This implementation also includes condensed list dependence models which, like condensed list relevance models, first conduct a query likelihood retrieval and then rerank the results using the dependence model.  This can substantially improve speed, especially for longer queries.  


For example, to run a classic SDM reranking of a length 100 query likelihood initial retrieval,
```
IndriRunQuery -index=/path/to/index -query="hello world" -dm=order:1,rerank:100
```

Dependence models are built after internally stopping and stemming the query terms.



## Citation 

Use the standard indri citation and  references to whatever algorithms above you are using.  Link to the code for reproducibility.   

## Notes

To build on OSX, be should to set `CXXFLAGS=-fno-tree-vectorize`.  Thanks to [Luke Gallagher](https://github.com/lgrz).  

