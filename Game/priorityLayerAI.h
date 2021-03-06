//--------------------------------------------------------------------------------------
//  優先度レイヤーAIクラス   ( priorityLayerAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PRIORITY_LAYER_AI_H_
#define _PRIORITY_LAYER_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <list>
#include <string>
#include "characterAI.h"
#include "layerAI.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class NodeAI;

//--------------------------------------------------------------------------------------
//  優先度レイヤーAIクラスの定義
//--------------------------------------------------------------------------------------
class PriorityLayerAI : public LayerAI
{
public:
	PriorityLayerAI( std::string name ,								//  引数付きコンストラクタ
					 CharacterAI* characterAI ,
					 NodeAI* parentNode ) :
					 LayerAI( CharacterAI::LAYER_TYPE::PRIORITY , 
							  name ,
							  characterAI ,
							  parentNode ){ }	  

	void			Run( void ) override;							//  実行	

private:

};

#endif
