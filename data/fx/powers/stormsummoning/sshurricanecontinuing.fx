#########################################################
##	template

FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Root
End

Input  
	InpName	Hips
End


#Lifespan 100



#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName  ChestNode
		Type	local
		Geom	RootToChestAdjustment
		At	Root
		Bhvr	behaviors/genericparticlefade.bhvr

	End

End

Condition
	On 	Time
	Time 	19

	Event
		EName 	FogRing1
		Type	Local 
		At	Hips
		#AltPiv	1
		bhvr    behaviors/SSSpin6.bhvr
		part	:SSHurricaneHit01.part
		part	:SSHurricaneHitChunks01.part
		#LifeSpan 120
		
	End

End


End		