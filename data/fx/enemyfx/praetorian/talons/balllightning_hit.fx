#############################################################
## BallLightning_Hit.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		part	:Electric_Ring_Arcs.part
		Sound elecballhit 60 60 .6
		lifespan 30
	End

	Event
		ENAME	H
		Type	start
		At	Root
		geom	roottochestadjustment
		lifespan 30
	End

	Event
		Type	start
		At	Chest
		altpiv	1
		Part1	:ElectricitySparkBurst.part
		Part2	:ElectricitySparkBurst.part
		lifespan 30
	End

	Event
		Type	Local
		At	Chest
		Part2	:ShockFieldGlow.part
		lifespan 5
	End
End

#############################################################

End