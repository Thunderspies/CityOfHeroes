#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition

	Event
		EName	JuggleRig
		Type	Local
		At	Mystic
		Anim	FX_JuggleBalls_Huge
	End
End

## BALLS ###########################################################

Condition
	On 	Time
	Time 	12

	Event
		EName 	Ball1
		Type	Local
		At	JuggleRig
		AnimPiv	Neck
		Geom	GEO_LavaBall
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplashEmbers.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Ball2
		Type	Local
		At	JuggleRig
		AnimPiv	Waist
		Geom	GEO_LavaBall
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplashEmbers.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Ball3
		Type	Local
		At	JuggleRig
		AnimPiv	Chest
		Geom	GEO_LavaBall
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplash.part
		Part	V_COV\PhysicsEnabled\FireSplashEmbers.part
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
	End
End

#############################################################

End