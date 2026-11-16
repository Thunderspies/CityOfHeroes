#############################################################
## JoltingChain_Hit.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Input
	Inpname	Chest
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:ElectricityGlow.part
		Part	:Hit_Flash_Big.part
		Part	:Hit_Flash_Big2.part
		Part	:Hit_Lightning_Streak.part
		Part	:Hit_Lightning.part
		Part	:ElectricityArch4a.part
		Part	:ElectricityArchSmalla.part
		Part	:Hand_Ring_Burst.part
		Part	:ElectricityArchContinuing.part
		Part	:ElectricityArchContinuing2.part
		Lifespan 30
		Sound	elecfry 80 70 .8
	End
End

Condition
	On 	Time
	Time 	0
	Repeat	3

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX		V_COV\PhysicsEnabled\LightningHit.fx

		Lifespan	90

	End
End

Condition
	On	Time
	Time	0
	Repeat	5

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSpark.fx

		Lifespan	90

	End
End


End
