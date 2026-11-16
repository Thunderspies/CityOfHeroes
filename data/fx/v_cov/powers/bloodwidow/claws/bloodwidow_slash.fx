#############################################################
## Blood Widow "Strike" FX
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade_R
		LifeSpan 50
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	WepL
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade
		LifeSpan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	Streak
		Type	local
		At	Mystic
#		Bhvr	behaviors\Claws.bhvr
#		Geom	swoosh
		Sound Claw2b 100.0 100.0 .6
		LifeSpan 2
	End
End

#############################################################

End
