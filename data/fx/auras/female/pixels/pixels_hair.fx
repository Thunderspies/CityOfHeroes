#############################################################
## Pixels_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 0 0
		End
		Part	:Pixel_Sm.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run.part
		Part	:Pixel_Run2.part
		Part	:Pixel_Run2.part
	End


	Event
		Type	Local
		At	Head
		Bhvr	Auras\Male\Glow\HeadOffset1.bhvr
		BhvrOverride
			PositionOffset	0 0.2 0
		End
		part	:Pixel_BodyGlow01.part
		Pmagnet Head
	End







End
#########################################################
End