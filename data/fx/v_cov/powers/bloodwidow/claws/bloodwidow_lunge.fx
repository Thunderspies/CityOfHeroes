#############################################################
## Blood Widow "Lunge" FX
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	WepR
		Sound claw1b 60 60 .5
	End

	Event
		Type	local
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade_R
		LifeSpan 40
	End
End

#############################################################

End