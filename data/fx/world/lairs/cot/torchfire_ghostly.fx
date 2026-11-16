#############################################################
## TorchFire_Ghostly.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part1	WORLD\Lairs\cot\Flame_Ghostly.part
		Part1	WORLD\Lairs\cot\Flame_Ghostly_Base.part
		Part1	WORLD\Lairs\cot\Flame_Ghostly_Glow.part
		Part3	WORLD\Lairs\cot\Sparks_Ghostly.part
		Sound Torch1_loop 22.0 22.0 1.0
	End
End

#########################################################

End