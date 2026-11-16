#############################################################
## Blood Widow "Strike" FX
#############################################################

FxInfo

Lifespan 45

#############################################################

Condition
	On 	Time
	Time 	0

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