#########################################################
##	template

FxInfo

#Lifespan 60



#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
#		Sound gravityzap2 80 80 .75
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	root
		part1	:GCBodyGlow.part
		part1	:GCBodyGlowMist.part
		part1	:GCHandOrbits.part
			Part 	:DimensionSphere_Border_Ground.part
			Part 	:DimensionSphere_Border.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
				#tintgeom 1
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
				#TintGeom 1
				StartColor 177 126 255
			End
			Geom Tintable_ShadowField
	End

End

End