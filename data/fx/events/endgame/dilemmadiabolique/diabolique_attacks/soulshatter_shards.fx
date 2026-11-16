###########################################################

FxInfo

	Flags DontSuppress
	Lifespan 300

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_SoulShatter_Shards_01 100 100 .8
	End
End

Condition
	On Time
	Time 	0

	Event
		Type	Start
		At	Root
		Part	:Soulshatter_Shards_Black.part
		Part	:Soulshatter_Shards.part
		Part	:Soulshatter_Smoke.part
		Part	:FrigidGrasp_DustPillar_Up.part
		Lifespan 2
	End

	Event
		Type	Start
		At	Head
		Part	:Soulshatter_Smoke_Explode.part
		Lifespan 2
	End
End

###########################################################

End