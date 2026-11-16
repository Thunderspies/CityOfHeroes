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


Lifespan 200



#########################################################
#####################  CLOUD 1  #########################
#########################################################

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	OverHead
		Type	Local
		At	Root
		#Geom	RoottoChestAdjustment
		#Lookat	Target
		Sound ss04 100 100 .8
	End

End

Condition

	On 	Time	
	Time	30
	
	Event	
		ENAME	Mist
		Type	Start
		At	OverHead
		#AltPiv	1
		part	:SSTornadoCastMist01.part
		part	:SSTornadoCastStreaks01.part
		part	:SSGaleChunks01.part
		#Bhvr	Behaviors/Spin2.bhvr
		LifeSpan 70
	End

	Event
		Ename	PushForce
		Type	Local
		
		At	OverHead
		
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	100
		End
		

		LifeSpan	75

	End

	
	Event
		Ename	PushForce2
		Type	Local
		
		At	OverHead
		
		BhvrOverride
			PositionOffset		0 0 37.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		50
			PhysForcePowerJitter	25
		End
		

		LifeSpan	75

	End
End



End		