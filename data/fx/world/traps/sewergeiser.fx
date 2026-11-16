#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo
lifespan 500

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Steam
#		Type	StartPositOnly
#		At	Root
#		BhvrOverride
#			PositionOffset	0 -2 0
#			Scale		1.25 1.25 1.25
#		End
#		Part	:Bottom_Foam_Flat.part
#		Part	:Bottom_Foam_Flat.part
#		Sound	Sewer_SewerBubble_Start_01 100 100 .8
#		Lifespan 200
#	End
#End
#
#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Steam
#		Type	StartPositOnly
#		At	Root
#		BhvrOverride
#			PositionOffset	0 0 0
#		End
#		ChildFX	:Sewer_Bubbles_Small.fx
#		Sound	Sewer_SewerBubble_Loop 100 100 .8
#		Lifespan 185
#	End
#End

#############################################################

Condition
	On	Time
	Time	00

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -2.5 0
			Scale		1.25 1.25 1.25
		End
		Part	:ViolentSplash_Initial.part
		Sound	Sewer_SewerGeiser_Start_01 100 100 .8
		Lifespan 5
	End
End

Condition
	On	Time
	Time	03

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 -2.5 0
			Scale		1.25 1.25 1.25
		End
		Part	:ViolentSplash_Initial.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	05

	Event
		Ename	Steam
		Type	StartPositOnly
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.5 0
			Scale		1.25 1.25 1.25
		End
		Part	:SteamAlpha.part
		Part	:ViolentSteamCore.part
		#Part	:ViolentSteamCore2.part
		Part	:ViolentSteamCoreAdd.part
		Part	:Top_Splash_Spray.part
		Part	:Top_Splash.part
		Part	:Splash_Base.part
		Part	:Splash_Base02.part
		Part	:Bottom_Splash_Secondary.part
		Part	:Bottom_Foam_Flat.part
		Part	:Bottom_Foam_Flat.part
		Sound	Sewer_SewerGeiser_Loop 100 100 .8
		#Lifespan 25
	End
End

#############################################################

End