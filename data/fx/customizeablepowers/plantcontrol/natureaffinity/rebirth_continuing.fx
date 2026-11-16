FXinfo
Lifespan 150

Condition
	On Time
	time 30

	Event
		Ename ConeAnchor
		Type Local
		At Root
		Geom FX_ConeOffsets
		BhvrOverride
			Scale 15 15 15
		End
	End

	Event
		Type Local
		At Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			FadeInLength	120
			PositionOffset	0 0 25
			Alpha		255
			StartColor	120 150 100
			#PrimaryTint	80
			#SecondaryTint	20
			Scale		70 20 45
			#SplatFlags	ADDITIVE
		End
		#Splat	Tintable_PlantGrowth_Cone
		#Lifespan 60
	End
#
#	Event
#		Type Local
#		At Root
#		Bhvr	behaviors\Splat_RuneCircle.bhvr
#		BhvrOverride
#			FadeInLength	120
#			#PositionOffset	0 0 25
#			Alpha		255
#			StartColor	80 180 75
#			#PrimaryTint	80
#			#SecondaryTint	20
#			Scale		45 20 45
#			#SplatFlags	ADDITIVE
#		End
#	#	Splat	Tintable_PlantGrowth_Round
#		#Lifespan 60
#	End
#
	Event
		Type PositOnly
		At ConeAnchor
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			FadeInLength	120
			#PositionOffset	0 0 25
			Alpha		255
#			StartColor	80 180 75
			PrimaryTint	80
			SecondaryTint	20
#			Scale		45 20 45
#			#SplatFlags	ADDITIVE
		End
		geom P_Bush_AzSmA
		#geom _P_Bush_PrivSmAFol
		Lifespan 60
	End
End

##############################################################


End