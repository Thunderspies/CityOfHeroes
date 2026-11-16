#########################################################
##	template

FxInfo
#Lifespan 10


#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	AltarTop
		Type	Local
		At	Origin
		part	:AltarTableMist01.part
		part	:AltarTableRings01.part
		part	:AltarTableMistFall02.part
		part	:AltarTableMistFall01.part
		bhvr	:AltartableOffset01.bhvr
	End
	
	Event	
		ENAME	AltarFloor
		Type	Local
		At	Origin
		part	:AltarFloorMist01.part
		part	:AltarRings01.part
		bhvr	:AltarOffset01.bhvr
	End


End


End		