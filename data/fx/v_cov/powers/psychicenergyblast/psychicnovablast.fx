#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	InpName	Root
End

Input  
	Inpname	hips
End

Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
		Sound Nova 100.0 100.0 .8
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		50
			PhysForcePowerJitter	10
		End

		Lifespan	50

	End
End

Condition

	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
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
	Time 	60

	Event
		EName 	Primex
		Type	Local 
		At	hips

		Part1	:InfernorockBits.part
		Part2	:InfernorockBits2.part
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	powers/firecontrol/Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		
	End		


End

Condition
	On 	Time
	Time 	58
	Event
		EName 	rings
		Type	Local 
		At	root
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	62

	Event
		EName	ring
		Type	Local
		At	RadiusFxScale
		geom	PsychicRing2
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
