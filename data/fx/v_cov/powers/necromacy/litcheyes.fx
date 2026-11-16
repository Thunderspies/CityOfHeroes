#############################################################
## LitchEyes.fx
#############################################################

FxInfo
Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns

	End

	Event
		Type	Local
		At	Prime
		 AltPiv 1
		 BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part1	:LitchEyes_Green.part
		Part2	:LitchEyes_Yellow.part
		Part3	:LitchEyesMistLeft.part
		Part3	:TendrilEyesMistLeft.part
	End

	Event
		Type	Local
		At	Prime
		 AltPiv 2
		Part1	:LitchEyes_Green.part
		Part2	:LitchEyes_Yellow.part
		Part3	:LitchEyesMistRight.part
		Part3	:TendrilEyesMistRight.part
	End

End

End
##################################
