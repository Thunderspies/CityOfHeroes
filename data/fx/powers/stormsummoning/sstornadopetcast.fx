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
	InpName	Chest
End

Input  
	InpName	WepL
End


Lifespan 100



#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	OverHead
		Type	Local
		At	Origin
		Bhvr	behaviors/genericparticlefade.bhvr
		Geom	OverHeadDummy
	End

End

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	Lightning
		Type	Start
		At	OverHead
		#AltPiv	1
		#part	:SSLSCloudLightning01.part
		#LifeSpan 10
	End

End

Condition

	On 	Time	
	Time	5
	
	Event	
		ENAME	Mist
		Type	Start
		At	OverHead
		AltPiv	1
		part	:SSTornadoCastMist.part
		#Bhvr	Behaviors/Spin2.bhvr
		PMagnet	target
		LifeSpan 20
	End

End

Condition

	On 	Time	
	Time	0
	
	Event	
		ENAME	Mist2
		Type	Start
		At	OverHead
		AltPiv	1
		part	:SSTornadoCastMist2.part
		Bhvr	Behaviors/Spin2.bhvr
		LifeSpan 15
	End

End



End		