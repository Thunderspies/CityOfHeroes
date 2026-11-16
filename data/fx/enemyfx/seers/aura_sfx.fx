#########################################################
## Seers Aura SFX
#########################################################

FxInfo

Condition

	Event
		Type Local
		At Origin
		Sound Seers_AuraTone_Loop 15 15 .15
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
	End
	
	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset 9 1 6
		End
		Sound Seers_AuraVoicesR_Loop 35 35 .2
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End

	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset -9 1 6
		End
		Sound Seers_AuraVoicesL_Loop 35 35 .2
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End
	
	Event
		Type Local
		At Origin
		BhvrOverride
			PositionOffset 0 1 -10
		End
		Sound Seers_AuraVoicesC_Loop 35 35 .2
		bhvr	behaviors/Sound/SoundIn1Out2.bhvr
		# Part flash.part
	End
End