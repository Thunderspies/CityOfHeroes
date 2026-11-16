#############################################################
## MarkedForDeath_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hair
		part	:Target_Reticle.part
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Part	:Target_Glow.part
	End
End

#############################################################

End