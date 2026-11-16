#########################################################
##	Fire_Ball
##
FxInfo

Flags InheritAlpha ###InheritAnimScale

#######
##Sound
#######################################################################

Condition

	On	Time
	Time	24

	Event
		Type	Local
		At	Origin
		Sound darkarmor2 60 60 .99
	End
End

################################################################
#Wings
################################################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	WingAnim
		Type	Local
		At	Back
		Anim	FX_wings_BURNED
		
#		BhvrOverride		
#			StartColor	10 10 10
#		End
#
		Bhvr	behaviors/WingScale_BURNED_Male_VFX_Wraith.bhvr
	End

End

##############################################################################################
##SmokeAura
##############################################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	Hips	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandL	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandR	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_R	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_L	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		Part	:BurntWingSmokeaura.part

	End

End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		Part	:BurntWingSmokeaura.part

	End
End

##########################################################



Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End

		Part	:BurntWingSmokeaura.part

	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End

		Part	:BurntWingSmokeaura.part

	End
End


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 1 -2.5
		End

		Part	:BurntWingSmokeaura.part

	End
End

#####################################################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End

		Part	:BurntWingSmokeaura.part

	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End

		Part	:BurntWingSmokeaura.part

	End
End


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 1 -2.5
		End

		Part	:BurntWingSmokeaura.part

	End
End

######################################################################################
##Licks of fire
########################################

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	LarmL	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	LarmR	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmL	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmR	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandL	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	HandR	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_R	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	T3_L	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End

##########################################################



Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End


Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingR	
		
		
		BhvrOverride
			PositionOffset	0 1 -2.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End

#####################################################################################

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 1.5 -3.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End

Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 0.5 -1.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End


Condition
	On 	cycle
	Time 	5
	Chance	.05

	Event
		Type	Local
		At	WingAnim
		AnimPiv	RingL	
		
		
		BhvrOverride
			PositionOffset	0 1 -2.5
		End

		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingFire.part
		Part	AnimatedCharacterParts/BurntWingSmoke.part
		Part	AnimatedCharacterParts/BurntWingEmber.part

		LifeSpan	5
	End
End

###############################################3
##End of Wings
##################################
##Beginning of wraith
##########################################################

Condition

	Event
		EName 	Prime
		Type	Local 
		At	Eyes
		Geom	FX_Daemon
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		BhvrOverride
			PositionOffset	.07 0 0
		End

		Part	:Leiutenant_Eyes.part
		Part	:Leiutenant_EyesMistRight.part
		Part1	:Leiutenant_DaemonPupil.part
		Part2	:Leiutenant_DaemonEyes.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		 
		BhvrOverride
			PositionOffset	-.07 0 0
		End

		Part	:Leiutenant_Eyes.part
		Part	:Leiutenant_EyesMistLeft.part
		Part1	:Leiutenant_DaemonPupil.part
		Part2	:Leiutenant_DaemonEyes.part
		
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	2
		Type	Local
		At	back
		part1	:Leiutenant_Smokex.part
		part2	:Smoke2x.part
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part

	End

	Event
		EName	3
		Type	Local
		At	waist
		part1	:Leiutenant_Smoke.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet Head

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Leiutenant_Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		Part	:Leiutenant_SmokeHead.part
#		part1	:Leiutenant_Smokex.part
#		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Leiutenant_Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Leiutenant_HandSmokeys.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet HandL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Leiutenant_Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Leiutenant_HandSmokeys.part
		part2	:Smoke2x.part
		
		Part5	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet HandR
	End

	Event
		EName	14
		Type	Local
		At	handL

		BhvrOverride
			Alpha	55
		end

		Geom	VFX_WraithHand01

		part1	:Leiutenant_HandSmokeys.part
		part2	:Smoke2x.part
		
		Part5	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet LarmL
	End

	Event
		EName	14
		Type	Local
		At	handR

		BhvrOverride
			Alpha	55
		end

		Geom	VFX_WraithHand

		part1	:Leiutenant_HandSmokeys.part
		part2	:Smoke2x.part
		
		Part5	:DarkBlastHitTendrilx.part
		Part	:DarkBlastHitTendrilx_Subtractive.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:Smoke2s_Subtractive.part
		Part	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		PMagnet LarmR
	End

	Event
		EName	2
		Type	Local
		At	LLEGR
		part1	:Leiutenant_SmokeTail.part

	End
End     

End	

