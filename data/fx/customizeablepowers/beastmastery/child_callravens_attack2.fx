#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo
Lifespan 200

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	:CallRavens_Projectile.bhvr
		BhvrOverride
			TintGeom	1
			PositionOffset	-0.0 4.0 -15.0
		End
		Anim	FX_Crow_Tintable
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	:CallRavens_Projectile.bhvr
		BhvrOverride
			TintGeom	1
			PositionOffset	-0.0 4.0 -15.0
		End
		Anim	FX_Crow_Tintable
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	:CallRavens_Projectile.bhvr
		BhvrOverride
			TintGeom	1
			PositionOffset	-0.0 4.0 -15.0
		End
		Anim	FX_Crow_Tintable
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	4

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	:CallRavens_Projectile.bhvr
		BhvrOverride
			TintGeom	1
			PositionOffset	-0.0 4.0 -15.0
		End
		Anim	FX_Crow_Tintable
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime
		Type	Local
		At	Root
		Bhvr	:CallRavens_Projectile.bhvr
		BhvrOverride
			TintGeom	1
			PositionOffset	-0.0 4.0 -15.0
		End
		Anim	FX_Crow_Tintable
		Lifespan 30
	End
End



#############################################################

End