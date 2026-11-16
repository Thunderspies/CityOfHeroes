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
		Sound Icebreak3 220.0 220.0 1.0
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		100
			PhysForcePowerJitter	25
		End

		Lifespan	15

	End
End

Condition

	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
		Part2	:Inferno.part
		#Sound	rumble7 220 30 .3

	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Primex
		Type	Local 
		At	hips

		Part1	:InfernorockBits.part
		Part2	:InfernorockBits2.part
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End

		Lifespan	1

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
	Time 	3
	Event
		EName 	Prime8
		Type	Local 
		At	root
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	17

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	behaviors/BlastRingScale2a.bhvr
		LifeSpan		1
	End

End

Condition
	On 	Time
	Time	32

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
	Time	32

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
