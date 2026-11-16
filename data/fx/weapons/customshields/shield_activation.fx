#############################################################
## Costume Anim Entity - Shield - Activation FX
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		EName	Sound1
		Type	local
		At	LArmL
		Sound Roman_soldier_energized_shield 100.0 100.0 .5
	End
#
#	Event
#		EName	Sound2
#		Type	local
#		At	LArmL
#		Sound	Roman_soldier_energized_shield_2 100.0 30.0 .5
#	End
#
#	Event
#		EName	Sound3
#		Type	local
#		At	LArmL
#		Sound	Roman_soldier_energized_shield_3 100.0 30.0 .5
#	End
End
#############################################################

End