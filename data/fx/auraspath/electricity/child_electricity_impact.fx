#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		part	:Rings_Expanding_Immediate.part
		part	:Electricity_Ground_Bolts_Immediate.part
		part	:Explosion_ElectricArcs.part
		part	:Explosion_Flash_Large.part
		part	:Explosion_Flash.part
		part	:Smoke_Immediate.part
		Part	:Electricity_Bolts_Broken_Immediate.part
		Lifespan 5
	End
End

#########################################################

End