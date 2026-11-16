#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	InpName	Root
End

Input  
	Inpname	Chest
End

Input  
	InpName	WepL
End

Input  
	Inpname	WepR
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Chest
		
		
		Part2	powers/fireControl/Inferno2.part
		Sound inferno4 80 80 .9

	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Primex
		Type	Local 
		At	Chest
		LifeSpan	10
		Part1	:CadavorBits.part		
		
	End

	Event
		EName 	Primex
		Type	Local 
		At	Chest
		Bhvr	Behaviors/CameraShake01.bhvr
		
		Part	WeaponFx/SmokeExplosion.part
		#Part2	powers/fireControl/InfernoExplosion.part
		Part	WeaponFx/fireExplosion2.part
		
	
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End

	Event
		EName   RadiusFxScale2
		Type	local
		At	Root
		Geom	RootToChestAdjustment
		
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale2
		Altpiv    1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part
		
	End		

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale2
		Altpiv    1
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		1000
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End

End


Condition
	On 	time
	Time 	0
	Repeat  9

	Event 
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event 
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
	
	Event 
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	hips
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
		
#	Event 
#		HardwareOnly
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstUP.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
End


Condition
	On 	Time
	Time 	7
	Event
		EName 	Prime8
		Type	Local 
		At	RadiusFxScale2
		Altpiv    1
		Part1   powers/firecontrol/Infernoflash.part
		
	End
End

Condition
	On 	Time
	Time	22

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
