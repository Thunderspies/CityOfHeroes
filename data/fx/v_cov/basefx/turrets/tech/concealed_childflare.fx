FxInfo

Lifespan 75

Condition
	On	Cycle
	Time	3

	Event
		EName	GunFlare
		Type	Local
		At	Origin
		bhvrOverride
			Behavior
			FadeInLength		0
			FadeOutLength		1
			PositionOffset		.25 -.1 0 
		End
		
		Bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_GunBurst.bhvr
		Part1	V_COV\BaseFx\turrets\Tech\Concealed_P_Flash.part
		Part2	V_COV\BaseFx\turrets\Tech\Concealed_P_Glow.part
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GlowNoV.part
		Lifespan 2
	End

	Event
		EName	GunFlare01
		Type	Local
		At	Origin
		bhvrOverride
			Behavior
			FadeInLength		0
			FadeOutLength		1
			PositionOffset		0 -.1 0 
		End
		
		Bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_GunBurst.bhvr
		Part1	V_COV\BaseFx\turrets\Tech\Concealed_P_GlowNoV.part
		Lifespan 2
	End
End

######################################################################################################
########################################   Flare Emitters   ##########################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	FlareP_01
		Type	Local
		At	Origin
		
		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_FlareParent01.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_02
		Type	Local
		At	Origin
		
		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_FlareParent02.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_03
		Type	Local
		At	Origin
		
		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_FlareParent03.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_04
		Type	Local
		At	Origin
		
		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_FlareParent04.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_05
		Type	Local
		At	Origin
		
		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_FlareParent05.bhvr
		Lifespan 100
	End
End

######################################################################################################
#########################################   Flare Fingers   ##########################################
######################################################################################################

Condition
	On	Cycle
	Time	3

	Event
		EName	MuzFlare01
		Type	Local
		At	FlareP_01

		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_Flare01.bhvr
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Lifespan 1
	End

	Event
		EName	MuzFlare02
		Type	Local
		At	FlareP_02

		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_Flare01.bhvr
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Lifespan 1
	End

	Event
		EName	MuzFlare03
		Type	Local
		At	FlareP_03

		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_Flare01.bhvr
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Lifespan 1
	End

	Event
		EName	MuzFlare04
		Type	Local
		At	FlareP_04

		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_Flare01.bhvr
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Lifespan 1
	End

	Event
		EName	MuzFlare05
		Type	Local
		At	FlareP_05

		bhvr	V_COV\BaseFx\turrets\Tech\Concealed_B_Flare01.bhvr
		Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Lifespan 1
	End

End

End