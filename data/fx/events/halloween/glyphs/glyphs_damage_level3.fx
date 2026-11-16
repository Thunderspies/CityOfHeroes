#############################################################
## Halloween_Sigil_Spawn.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	SigilFace
		Type	Local
		At	Root
		Bhvr	behaviors\fadein.bhvr
		BhvrOverride
			PositionOffset	0 0.1 0
			Scale		60 60 60
		End
		Geom	FX_SigilOffsets
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName	flash
		Type	Local
		At	SigilFace
		AltPiv	1
		BhvrOverride
			StartColor	255 50 50

			PositionOffset	0 0.005 0
			Scale 		30 30 30
		End
		Geom	FX_Sigil_TT_1
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName	flash
		Type	Local
		At	SigilFace
		AltPiv	2
		BhvrOverride
			StartColor	255 50 50

			PositionOffset	0 0.005 0
			Scale 		30 30 30
		End
		Geom	FX_Sigil_TT_1
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName	flash
		Type	Local
		At	SigilFace
		AltPiv	3
		BhvrOverride
			PositionOffset	0 0.005 0
		End
		Part	:Sigil_Ring_Buff_DF.part
		Part	:Sigil_Ring_Buff_Expanding_DF.part
		ChildFX	WORLD\Lairs\cot\Geysers_Damage.fx
		Lifespan 20
	End

	Event
		EName	flash
		Type	Local
		At	SigilFace
		AltPiv	3
		BhvrOverride
			StartColor	255 50 50

			PositionOffset	0 0.005 0
			Scale 		30 30 30
		End
		Geom	FX_Sigil_TT_1
	End
End

#########################################################

End