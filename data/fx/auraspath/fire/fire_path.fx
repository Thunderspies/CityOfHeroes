#############################################################
## RoofFire.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	root


		Sound buildingfire3_loop 180 180 .89
	End

	Event
		Type	Local
		At	root
		BhvrOverride
		#	InitialVelocity	0.0 1.0 0.0
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Flame_Small_Root.part
		Part	:Flame_Small_Root_Alpha.part
		Part	:RoofFireSpark.part
		Part	:Flame_Small_Root_Stationary.part
		Part	:Flame_Small_Spread_Stationary.part
		Part	:Flame_Small_Spread_Alpha_Stationary.part
		Part	:Flame_Small_Root_Alpha_Stationary.part
		Part	:RoofFireSpark_Stationary.part
		Sound buildingfire3_loop 180 180 .89
	End
End

#############################################################

End