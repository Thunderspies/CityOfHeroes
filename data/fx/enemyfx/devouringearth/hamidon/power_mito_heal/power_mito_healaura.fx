#########################################################
## Mitchondria - Healing Aura
#########################################################

FxInfo

LifeSpan	60

#########################################################

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Hips

		Sound darkregen 66 66 .5
	End
End

Condition
	On	Time
	Time	7
	Event
		EName	Burst
		Type	local
		At	Hips
		Sound radcharge 66 66 .55
	End
End

Condition
	On	Time
	Time	21
	Event
		EName	Burst2
		Type	local
		At	Hips
		Sound radiation5_loop 66 66 .75
	End
End

#########################################################

Condition
	On	Time
	Time	0
	Event
		EName	Rings
		Type	local
		At	Hips
		Part1	:Power_Mit_HealAura_LightRays.part
		Part2	:Power_Mit_HealAura_Rings.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Rings
		Type	local
		At	Hips

		Part4	:Power_Mit_HealAura_RingsFlat.part
	End
End

#########################################################

End				