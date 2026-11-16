#############################################################
## Protean_PowerSiphon.Fx
#############################################################

FxInfo
LifeSpan	125 #90

#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### AOE Indicator ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Protean_SiphonTell_01 200 200 1
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_CircleGlow.part
		LifeSpan	90
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_CircleShrinking.part
		LifeSpan	50
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Chest
		BhvrOverride
			Shake          .3
			ShakeFallOff   .005
			ShakeRadius    180.0
			PositionOffset	0 .1 0

		End
		Part	:Chest_Headlight.part
		LifeSpan	40
	End



	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Body_Glow.part

		Pmagnet SpadL
		POther  SpadL
		Part	:Body_Glow.part
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		Part	:Body_Glow.part

		Pmagnet SpadR
		POther  SpadR
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		Part	:Body_Glow.part
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		Part	:Body_Glow.part


		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Body_Glow.part


		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part	:Body_Glow.part


		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Body_Glow.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Body_Glow.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Part	:Body_Glow.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Body_Glow.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Part	:Body_Glow.part
		Pmagnet LlegR
		POther  LlegR
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Root_CircleShrinking2.part
		Part	:Root_CircleStatic.part

		LifeSpan	50
	End
End

##########################################

End