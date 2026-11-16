#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90



#Condition
#	On	Time
#	Time	32
#
#	Event
#		Type	Local
#		At	Origin
#		Sound	FireZap2 80 67 .5
#	End
#End

Condition
	On 	triggerbits
	Triggerbits	death



	Event
		EName 	Primex
		Type	start 
		At	chest
		
		Part2	:TimeBombInferno.part
		Sound inferno4 80 80 .9

	End

	Event
		EName 	Primex
		Type	start 
		At	chest
		LifeSpan	10
		Part1	:TimeBombBits.part		
		
	End

	Event
		EName 	Primex
		Type	start 
		At	chest
		Bhvr	Behaviors/CameraShake01.bhvr
		
		Part	:SmokeExplosion.part
		Part	:fireExplosion2.part
		
	
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		800
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

	Event
		EName   RadiusFxScale
		Type	start
		At	chest
		
		Bhvr	:Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	start 
		At	chest
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/InfernoYellow2.part
		Part2	powers/firecontrol/InfernoYellowFlame01.part
		
	End		

	Event
		EName 	Prime8
		Type	start 
		At	chest
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
