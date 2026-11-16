#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On Cycle
	Time 5
	Repeat 2
	RepeatJitter 1

	Event
		At Root
		Type Local
		EName Splashes
		BhvrOverride
			StartJitter 20 0 20
		End
		Part WORLD\Weather\Rain\RainDrop_Splash_Fast.part
		LifeSpan 1
	End
End

End