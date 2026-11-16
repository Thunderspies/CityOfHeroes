#############################################################
## Electric_Hit.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Input
	Inpname	Chest
End


Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part	:Hit_Lightning.part
		Part	:Hit_Lightning2.part
		Part	:ElectricityArch4a.part
		Part	:ElectricityArchSmalla.part
		Lifespan 30
		Sound	EC_JoltingChain_Hit_01 100 100 .8
	End
End

#############################################################

End
