#############################################################
## Death_Gremlin.fx
#############################################################

FxInfo
LifeSpan 50

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		Part	:Death_Shockwave.part
		Part	:Death_Electricity_Arcs.part
		Part	:Death_Electricity_Arcs2.part
		Part	:Death_Sparks.part
		Part	:Hit_Flash_Big.part

		Part	:Hit_Flash_Big2.part
		Sound EC_Gremlins_Death_01 100 100 .8
		Lifespan 20
	End
End


Condition
	On	Time
	Time	0
	Repeat	1

	Event
		Type	Local
		At	Hips
		ChildFX	:Child_ElectricSparkHit.fx
		Lifespan 50
	End
End

#############################################################

End