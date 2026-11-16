#############################################################
## Blood Widow "Lunge" FX
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade_R
		LifeSpan 30
	End
End

#############################################################

End