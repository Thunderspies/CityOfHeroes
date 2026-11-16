#############################################################
## EyeOfTheStorm.fx
#############################################################

FxInfo
	Flags DontSuppress

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_EyeOfTheStorm_Loop 100 100 .8
	End
End

Condition
		On Time
		Time 0

	Event
			At Root
			Type Start
			EName EyeAnchor
			Part 	ENEMYFX\Praetorian\MoM\EyeOfTheStorm_Border_Ground.part
			Part 	:EyeOfTheStorm_Border.part
	End

	Event
			At EyeAnchor
			Type PositOnly
			EName StormGeo
			BhvrOverride
				PyrRotate 0 0 20
				Scale 7.3 7.3 7.3
				Spin 0 -0.225 0
				Alpha 60
				FadeInLength 20
				FadeOutLength 20
				StartColor 50 50 50
			End
			Geom Tintable_ShadowField_Highlights
	End

	Event
			At EyeAnchor
			Type PositOnly
			EName StormSwirls
			BhvrOverride
				PyrRotate 0 0 10
				Scale 7.25 7.25 7.25
				Spin 0 -0.2 0
				Alpha 60
				FadeInLength 20
				FadeOutLength 50
				StartColor 177 126 255
			End
			Geom Tintable_ShadowField
	End
End

Condition
		On Death

	Event
			Type Destroy
			EName CrushingHateGeo
		End

	Event
			Type Destroy
			EName CrushingHateSwirls
		End
	End
End
