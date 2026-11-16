#############################################################
## Cutscene Sound Effects
#############################################################

FxInfo

Lifespan 1

Condition

	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset 10 0 0
		End
		Sound Resistance-Ghouls_Battle1L_End 500 500 1
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End

	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset -10 0 0
		End
		Sound Resistance-Ghouls_Battle1R_End 500 500 1
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End
	
End