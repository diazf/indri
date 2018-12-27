indri
=====

This is a clone of Indri 5.12 with minor customizations.  

## IndriRunQuery customizations

Unless noted, all parameters are thrown on the command line just like other IndriRunQuery parameters. 

***NB. Input queries are assumed to be natural language, not Indri query language.***

### [Condensed List Relevance Models](https://dl.acm.org/citation.cfm?id=2808194.2809491)

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| rerankSize | int | 0 | relevance model reranks a length `rerankSize` initial query likelihood retrieval.  |

Using condensed list relevance models can substantially improve speed without degrading effectiveness of relevance models.  This means you can run massive query expansion very quickly.

### [External Expansion](https://dl.acm.org/citation.cfm?doid=1148170.1148200)

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| externalIndex | path | NONE | RM is built from an initial query likelihood retrieval from `externalIndex`.  |

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

### [Passage Retrieval]

| parameter | type | default | description |
| --------- | ---- | :-------: | ----------- |
| passageLength | int | 0 | length of passages to retrieve.  |
| passageOverlap | int | 0 | passage overlap.  |
| fbRankDocuments | bool | false | used in conjunction with `passageLength` > 0 and PRF for document ranking based on [passage RMs](https://dl.acm.org/citation.cfm?id=584854).  |




## Baseline Cheatsheet 

Here's a simple cheatsheet for good baselines for your text retrieval experiments.  I'm assuming that you are evaluating on a target corpus (e.g. Associated Press documents) and might either train your model with documents from the target corpus or from some large external corpus (e.g. Gigaword).  You also might incorporate term proximity or other  linear term relationships (e.g. n- or skip-grams).  

<table style="text-align:center">
  <tr>
    <td></td>
    <td colspan="3">training corpus</td>
  </tr>
  <tr>
		<td>proximity</td>
		<td>none</td>
		<td>target</td>
		<td>external</td>		
  </tr>
  <tr>
		<td>no</td>
		<td>QL</td>
		<td>RM3</td>
		<td>EE</td>		
  </tr>
  <tr>
		<td>yes</td>
		<td>DM</td>
		<td>DM+RM3</td>
		<td>DM+EE</td>		
  </tr>
</table>

For whatever the appropriate baseline is for your experiment, you should train the baseline's parameters on the same training set you're using for your model.  For RM3/EE, good ranges are,

| parameter | min | max |
| --------- | --- | --- |
| fbDocs | 5 | 500 |
| fbTerms | 5 | 500 |
| fbOrigWeight | 0 | 1 |

Don's original DM weights seem to be robust across conditions but you may still want to play with the parameters above.  

If you find DM/RM3/EE slow, instead of limiting the parameter ranges, you should opt for the condensed list approaches of the methods.  In general, you can rerank 1000 documents from a QL initial retrieval and preserve rank-equivalence with re-retrieval, especially at the top of the final ranking.  

If you are not sure where your model fits in the cheatsheet above, a strong baseline is DM+EE.  

**Note to paper readers (including reviewers):** You'll come across papers that use poorly implemented pseudo-relevance baselines.  Here are common mistakes,
* no parameter tuning (i.e.  a fixed number of feedback documents, expansion terms, or interpolation weight). 
* poor parameter tuning (e.g. only testing 5-25 terms and/or documents).  I've seen effectiveness peak at hundreds of documents/terms for some collections.
* unweighted document feedback (i.e. treating all feedback documents as equally relevant).  
* unweighted term expansion (i.e. treating the new terms just additional query terms).

## Citation 

Use the standard indri citation,
```
@inproceedings{strohman:indri,
author = {Trevor Strohman and Donald Metzler and Howard Turtle and W. B. Croft},
booktitle = {Proceedings of the International Conference on Intelligence Analysis},
title = {Indri: A language model-based search engine for complex queries},
year = {2004}
}
```
as well as the following for reproducibility,
```
@online{diaz:indri,
author = {Fernando Diaz},
title = {indri},
year = {2018},
url = {https://github.com/diazf/indri}
}
```
and references to any specific algorithms above you are using.

## Notes

To build on OSX, be should to set `CXXFLAGS=-fno-tree-vectorize`.  Thanks to [Luke Gallagher](https://github.com/lgrz).  

