#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Root
End

#Lifespan	225

##################################

Condition
	On	Time
	Time	0	

	Event
		EName	OverHeadNode
		Type	posit
		At	Root
		Geom	OverHeadDummy01
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		#Sound	coldblast2 100 30 .9
	End

End

Condition
	On	Time
	Time	0	

	Event
		EName	Snow01
		Type	local
		At	OverHeadNode
		part	:SSSnow01.part
		#bhvr    behaviors/SSSpin4.bhvr
		AltPiv	1
	End

	Event
		EName	Snow01
		Type	local
		At	OverHeadNode
		part	:SSSnow02.part
		#bhvr    behaviors/SSSpin5.bhvr
		AltPiv	1
	End

End

Condition
	On	Time
	Time	20	

	Event
		EName	Snow01
		Type	posit
		At	Root
		part	:SSSnowStormMist01.part
		bhvr    behaviors/SSSpin1.bhvr
		#Sound	cold_loop 100 30 .8
	End

End

Condition
	On	Time
	Time	8	

	Event
		EName	Snow01
		Type	posit
		At	Root
		part	:SSSnowStormMist02.part
		bhvr    behaviors/SSSpin2.bhvr
	End

	Event
		EName	Snow01
		Type	posit
		At	Root
		part	:SSSnowStormMist03.part
		bhvr    behaviors/SSSpin3.bhvr
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		CWSwirl
			PhysForceRadius		25
			PhysForceCentripetal	.25
			PhysForcePower		40
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
			PhysForceRadius		30
			PhysForcePower		40
			PhysForcePowerJitter	5
		End

		#Lifespan	30

	End
End


End		