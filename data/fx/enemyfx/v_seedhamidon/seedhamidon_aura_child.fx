#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#############################################################

Condition
	On 	Cycle
	Time 	500

	Event
		Ename	Indicator
		Type	Local
		At	Origin
		Bhvr	Behaviors\Braz\GeoBurst.bhvr
		BhvrOverride
			#Startcolor 	255 255 255
			Scale		8.6 8.6 8.6
			#Alpha		60
			FadeInLength	120
		End
		Geom	SeedInfluenceRad
		Lifespan 400
	End
End

#############################################################
End