
FxInfo

Flags	InheritAnimScale

Lifespan 300


#########  sound  ######################################



####### duffelbag landing target  ######################


Condition
	On	Time
	Time	0

	Event
		EName	CatchTarget
		Type	Local
		At	T_Root
		##Geom	Testing_Target
				Bhvr	Behaviors/GenericParticleFade.bhvr
                        BhvrOverride
                                    Behavior
                                    PositionOffset		.3 1 2
			End
		
	End
End



######  Duffelbag in hand  (pre toss)   #############################################

Condition
	On	Time
	Time	11

	Event
		EName	Hand_prop
		Type	Local
		At	WepR

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumAppear.bhvr
		Geom	duffelbag_hand
		Lifespan	24
	End
End


######  Fake projectile A (This will trigger hit react to simulate anticipation)  #############################################

Condition
	On	Time
	Time	15   ##1

	Event
		EName	Prime
		Type	Start
		At	WepR

		bhvr	V_COV\Powers\Thugs\Duffelbag_Travel_fake.bhvr
		##Geom	Testing_Target
		Magnet	CatchTarget #Target
		LookAt	CatchTarget
	End

End

######  Fake projectile B (This will trigger a fade out for the actual Projectile)  ############################

Condition
	On	Time
	Time	28  ##20

	Event
		EName	Prime2
		Type	Start
		At	WepR

		bhvr	V_COV\Powers\Thugs\Duffelbag_Travel_fake.bhvr
		##Geom	Testing_Target
		Magnet	CatchTarget #Target
		LookAt	CatchTarget
	End

End

#### Duffelbag projectile  ################################


Condition
	On	Time
	Time	36


	Event
		EName	Prime1
		Type	Start
		At	WepR

		bhvr	V_COV\Powers\Thugs\Duffelbag_Travel.bhvr
		Geom	duffelbag_projectile
		Magnet	CatchTarget

	End
End



###  impacts for Prime & Prime1  #################################


Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End



End


Condition
	On	Prime2Hit

	Event
		EName	Prime1
		Type	Destroy
	End


End

End