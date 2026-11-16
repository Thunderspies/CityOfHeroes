#############################################################
## Hit_Body_Antimatter.fx
#############################################################

FxInfo
LIfespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hair
		BhvrOverride
			PyrRotate	70 0 180
			PositionOffset	0 -1.5 1.5
		End
		Part	:Buildup_Additive.part
		Part	:Buildup_Additive2.part
		Lifespan	30
	End
	Event
		Type	Local
		At	Root
		Sound 	Hamidon_Cripple_01 200 200 .8
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hair
		BhvrOverride
			PyrRotate	70 0 180
			PositionOffset	0 -1.5 1.5
		End

		Part	:Shockwave_Acid.part
		Part	:Buildup_Splash.part
		Part	:Buildup_Splash_Additive.part
		Lifespan	50

	End
End

Condition

	On 	Time
	Time	25

	Event
		Type	Local
		At	Hair
		BhvrOverride
			PyrRotate	70 0 180
			PositionOffset	0 -1.5 1.5
		End

		Part	:Buildup_Flash.part
		Lifespan	50

	End

	Event
		EName 	Prime
		Type	Start
		At	Hair
		BhvrOverride
			InitialVelocity	0.0 0.25 0.0
			Gravity		0.005
			TrackRate	4.0
		End
		Part	:Projectile_Bright_Head.part
		Part	:Projectile_Body_Additive.part
		Part	:Projectile_Body.part
		Part	:Projectile_Smoke.part
		Magnet	T_Chest
		Lookat	T_Chest

	End
End

#########################################################

Condition

	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#########################################################

End