#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	Inpname	Origin
End

Input  
	Inpname	Hips
End

Input  
	Inpname	root
End

Condition

	On Time 
	Time 0
	
	Event
		Type	Start
		At	Origin
		Sound Nova 100.0 100.0 .8
	End

End

Condition

	On 	Time
	Time 	1

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
		At	Hips
		
		BhvrOverride
			PositionOffset		0 -1 0
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1250
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	6

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
		At	hips
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		
	End		


End

Condition
	On 	Time
	Time 	4
	Event
		EName 	Prime8
		Type	Local 
		At	hips
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End

End

Condition
	On 	Time
	Time 	8

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	behaviors/BlastRingScale2.bhvr
	End

End

Condition
	On 	Time
	Time	33

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
	Time	33

	Event
		EName 	Prime1
		Type	Destroy
	End

End	

End
