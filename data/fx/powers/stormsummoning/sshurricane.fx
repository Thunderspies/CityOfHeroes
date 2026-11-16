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
	InpName	Hair
End



#Lifespan 300



#########################################################
#####################  Mist 1  #########################
#########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName   RingNode
		Type	local
		Geom	RootToChestAdjustment
		At	Root
		Bhvr	behaviors/genericparticlefade.bhvr
		Sound coldblast2 100 100 .8

	End

End

Condition
	On 	Time
	Time 	19

	Event
		EName 	FogRing1
		Type	Local 
		At	RingNode
		AltPiv	1
		bhvr    behaviors/SSSpin3.bhvr
		#Part	:SSHurricaneRing02.part
		Part	:SSHurricaneRingSmall.part
		#LifeSpan 120
		Sound hurricane_loop 100 100 .7
		
	End

	Event
		Ename	SwirlForce
		Type	Local
		At	RingNode
		AltPiv	1
		
		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		30
			PhysForceCentripetal	.6
			PhysForcePower		50
			PhysForcePowerJitter	5
		End

		#Lifespan 40

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		16
			PhysForcePower		45
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End

	
	Event
		Ename	LevitationForce
		Type	Local
		At	RingNode
		AltPiv	1
		
		BhvrOverride
			PositionOffset		0 6 0
			PhysForceType		Up
			PhysForceRadius		10
			PhysForcePower		-15
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End
End
	
Condition
	On 	Time
	Time 	20
	
	Event
		EName 	Hurricane
		Type	Local 
		At	RingNode
		AltPiv	1
		Part	:SSHurricane01.part
		#LifeSpan 120
		
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	FogRing1
		Type	Local 
		At	RingNode
		AltPiv	1
		bhvr    behaviors/SSSpin1.bhvr
		Part	:SSHurricaneRing.part
		#LifeSpan 120
		
	End

End


End



	