#############################################################
## Hit_Body_Antimatter.fx
#############################################################

FxInfo
LIfespan 100
#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
	#	Sound eleccage_loop 70 70 .25
	End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################
	Event
		EName 	Prime
		Type	start
		At	T_Chest
		Bhvr	CustomizeablePowers\DarkMiasma\SoulTransfer1.bhvr
		BhvrOverride
			TrackRate	1.5
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body.part
		Magnet	Chest
		Lookat	Chest

	End

	Event
		EName 	Prime2
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset	0.0 0.0 2
		End
		Part	:Projectile_Body_Fill.part
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
		Part	:Disintegration_Hit.part
		LifeSpan	3

	End

	Event
		Type	LOCAL
		At	LLegR
		Part	:Glow_Bright.part
		POther	FootR

	End

	Event
		Type	LOCAL
		Bhvr	behaviors/GenericParticleFade.bhvr
		At	LLegL
		Part	:Glow_Bright.part
		POther	FootL

	End
End

Condition
	On	Time
	Time	6


	Event
		Type 	Local
		At 	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	ULegL
	End


	Event
		Type 	Local
		At 	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	ULegR
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Large.part
	End
End

Condition
	On	Time
	Time	18

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	LArmL

	End

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	LArmR
	End
End

Condition
	On	Time
	Time	24

	Event
		Type	LOCAL
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	WepL
	End

	Event
		Type	LOCAL
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	WepR
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	LOCAL
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Glow_Bright.part
		POther	Hair
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy
	End
End

#########################################################

End