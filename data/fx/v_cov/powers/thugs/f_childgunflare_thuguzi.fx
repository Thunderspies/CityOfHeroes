FxInfo

Lifespan 50

Condition
	On	Time
	Time	0
	
	Event
		EName	Emitter
		Type	Local
		At	Origin
		
		Bhvr	V_COV\Powers\Thugs\B_Emitter_GunBurst.bhvr
#		Geom	Testing_TargetB
	End
End

Condition
	On	Cycle
	Time	3

	Event
		EName	GunFlare
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0 0 0
		End
		
		Bhvr	V_COV\Powers\Thugs\B_Emitter_GunBurst.bhvr
		Part1	V_COV\Powers\Thugs\P_Spark_SmallStar.part
		Part2	V_COV\Powers\Thugs\P_Flare_MuzzleMiddle.part
#		Geom	Testing_TargetB

		Lifespan 2
	End

	Event
		EName	GunFlare01
		Type	Local
		At	Origin
		
		Bhvr	V_COV\Powers\Thugs\B_Emitter_GunBurst.bhvr
		Part1	V_COV\Powers\Thugs\P_GunFlash_White.part
		Lifespan 2
#		Geom	Testing_TargetB
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
		At	Emitter
		
		bhvr	V_COV\Powers\Thugs\B_FlareAngle01.bhvr
#		Geom	Testing_TargetB
		Lifespan 100
	End

	Event
		EName	FlareP_02
		Type	Local
		At	Emitter
		
		bhvr	V_COV\Powers\Thugs\B_FlareAngle02.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_03
		Type	Local
		At	Emitter
		
		bhvr	V_COV\Powers\Thugs\B_FlareAngle03.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_04
		Type	Local
		At	Emitter
		
		bhvr	V_COV\Powers\Thugs\B_FlareAngle04.bhvr
		Lifespan 100
	End

	Event
		EName	FlareP_05
		Type	Local
		At	Emitter
		
		bhvr	V_COV\Powers\Thugs\B_FlareAngle05.bhvr
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

		bhvr	V_COV\Powers\Thugs\B_Flare_Angle.bhvr
		Part1	V_COV\Powers\Thugs\P_GunFlare_Fingers.part
		Lifespan 1
	End

	Event
		EName	MuzFlare02
		Type	Local
		At	FlareP_02

		bhvr	V_COV\Powers\Thugs\B_Flare_Angle.bhvr
		Part1	V_COV\Powers\Thugs\P_GunFlare_Fingers.part
		Lifespan 1
	End

	Event
		EName	MuzFlare03
		Type	Local
		At	FlareP_03

		bhvr	V_COV\Powers\Thugs\B_Flare_Angle.bhvr
		Part1	V_COV\Powers\Thugs\P_GunFlare_Fingers.part
		Lifespan 1
	End

	Event
		EName	MuzFlare04
		Type	Local
		At	FlareP_04

		bhvr	V_COV\Powers\Thugs\B_Flare_Angle.bhvr
		Part1	V_COV\Powers\Thugs\P_GunFlare_Fingers.part
		Lifespan 1
	End

	Event
		EName	MuzFlare05
		Type	Local
		At	FlareP_05

		bhvr	V_COV\Powers\Thugs\B_Flare_Angle.bhvr
		#Part3	V_COV\BaseFx\turrets\Tech\Concealed_P_GunFlare.part
		Part1	V_COV\Powers\Thugs\P_GunFlare_Fingers.part
		Lifespan 1
	End

End

End