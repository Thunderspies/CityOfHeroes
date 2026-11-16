#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Indicator
		Type	PositOnly
		At	Root
		Bhvr	Behaviors\Braz\GeoBurst.bhvr
		BhvrOverride
			#Startcolor 	255 255 255
			Scale		8.6 8.6 8.6
			#Alpha		60
			FadeInLength	120
		End
		Geom	SeedInfluenceRad
	End
End


#############################################################
End