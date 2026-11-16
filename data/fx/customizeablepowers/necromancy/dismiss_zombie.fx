#############################################################
## Dismiss_zombie.fx
#############################################################

FxInfo
lifeSpan	110

############################################################

Condition
	On 	Time
	Time 	60

	Event

		ENAME	Base
		Type	local
		At	root

		Bhvr 	V_COV\Powers\Necromacy\Dismiss_DarkScale3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
		Sound miasmaburst 100 100 .6

		lifeSpan	10
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Part	:ScatterBones.part
		Part	:ScatterBones1.part
		Part	:ScatterBones2.part
		Bhvr	behaviors\bones_offset.bhvr
		lifeSpan	9
	End


End


Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	Root
		Part	:ScatterBones.part
		Part	:ScatterBones1.part
		Part	:ScatterBones2.part
		Bhvr	behaviors\bones_offset_2.bhvr
		lifeSpan	9
	End


End

############################################################

End