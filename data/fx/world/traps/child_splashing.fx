#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	SplashAnchor1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 5 10
		End
	End
End



Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0 0.0
		End
		Part	:Bottom_Splash_Large.part
		Part	:Bottom_Splash.part
		Part	:Splash_Base.part
		Part	:Splash_Base02.part
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0 0.0
		End
	#	Part	:Bottom_Splash_Secondary.part
	#	Part	:Splash_Thick01.part
	#	Part	:Splash_Thick02.part
	#	Part	:Splash_Thick03.part
		Lifespan 2
	End
End


#############################################################

End