#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		part	:Gnats.part
		Pmagnet	GnatAnchor
		Sound GnatSwarm_Loop 35 35 .15
	End
End

#############################################################

End