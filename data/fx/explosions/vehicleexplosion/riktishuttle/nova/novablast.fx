#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90
#################################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		BHVR	Behaviors/OffsetEyes.bhvr
		Geom	CircleOfThorns
		sound	peaceeyes1 60 50 .7
		LifeSpan	90
	End

	Event
		Type	Local 
		At	Primea
		 AltPiv 1
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
	
		LifeSpan	90
	End
	
	Event
		Type	Local 
		At	Primea
		 AltPiv 2
		BHVR	Behaviors/GenericparticleFade.bhvr
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
		LifeSpan	90
	End

End
######################################################################
Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
		Sound Nova4 100.0 100.0 .98
	End


	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		25
			PhysForcePowerJitter	10
		End

		Lifespan	50

	End
End

Condition

	On 	Time
	Time 	47

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		Part2	:Inferno.part

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	52

	Event
		EName 	Primex
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
		Part	:NovaTendrils.part
		Part	:InfernorockBits2.part
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:PeaceBringerTest.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:NovaSmoke.part
		Part	:NovaSmoke01.part
		Part	:NovaSmoke02.part
	End		


End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime8
		Type	Local 
		At	root
		bhvr	:BlastRingScale.bhvr
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	54

	Event
		Type	Posit
		At	HIPS
		Bhvr 	Powers/cosmicpowers/PeaceBringer/Nova/BlastScaller.bhvr
		
		Geom	CosmicPeaceBringerShield
		LifeSpan	17
				
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2a.bhvr
		LifeSpan		2
	End

End

Condition
	On 	Time
	Time	87

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	87

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
