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
			PositionOffset 50 0 0
		End
		Sound Resistance-PPD_Battle1L_End 1000 500 1
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End

	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset -50 0 0
		End
		Sound Resistance-PPD_Battle1R_End 1000 500 1
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End
	
End