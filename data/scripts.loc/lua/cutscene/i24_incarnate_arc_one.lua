cutsceneStart = [====[
			{
						< 0 SoundFX
							Music i24_Incarnate_Hamidon_Attack_CutsceneMusic_01
						>
						< 0 Actor1 ""
							Action Fly(1)
						>
						< 0 Actor1 ""
							Action DoNotFaceTarget
						>
						< 0 Actor1 ""
							Action BoostSpeed(0.5)
						>
						< 0 Actor16 ""
							Action Invincible(1)
						>
						< 0 Actor17 ""
							Action Invincible(1)
						>
						< 0 Actor16 ""
							Action Untargetable(0)
						>
						< 0 Actor17 ""
							Action Untargetable(0)
						>
						< 0 Actor2 "<em StanceHero2>"
						>
						< 0 Camera
							Position Camera2
							DepthOfField 17
						>
						< 3 Caption "<x=0.5 y=0.2 t=5 d=5><scale 1.75><color Silver><bgcolor DimGray>We have a visual on the Seed! Its too far for the turrets!"
						>
						< 2 Actor1 "<goto Move2>"
						>
						< 0 Camera
							Position Camera12
							DepthOfField 700
						>
						< 5 Caption "<x=0.5 y=0.2 t=6 d=5><scale 1.75><color White><bgcolor DarkBlue>This world has been scarred enough by the sin of humans."
						>
						< 5 Caption "<x=0.5 y=0.2 t=6 d=5><scale 1.75><color White><bgcolor DarkBlue>Emperor Cole had one chance, and he wasted it."
						>
						< 0 Camera
							Position Camera12
							DepthOfField 15
						>
						< 3 Actor2 "<em attack t=3 d=3>Fire!"
						>
						< 0 Camera
							Position Camera1
							MoveTo Camera3
							MoveToDepthOfField 100
							Over 5
						>
			]====]

cutsceneStartB = [====[
			{
			]====]



cutsceneFire = [====[
						< 0.3 Actor4 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor8 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor5 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor7 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor9 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor6 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor15 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor12 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0.3 Actor14 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor11 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0.3 Actor13 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor10 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0.3 Actor14 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0 Actor11 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0 Actor6 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0.3 Actor15 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0 Actor10 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0.3 Actor7 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor16
						>
						< 0.3 Actor9 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
						< 0 Actor13 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
						< 0.3 Actor12 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
						< 0 Actor8 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
						< 0.3 Actor5 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
						< 0 Actor4 ""
							Action Power:Plasma_Cannon_Barrage
							 Target Actor17
						>
			]====]

cutsceneCamera4 = [====[
			{
						< 0 Camera
							Position Camera4
							DepthOfField 150
						>
			]====]

cutsceneCamera5 = [====[
			{
						< 0 Camera
							Position Camera5
							DepthOfField 100
						>
			]====]

cutsceneCamera6 = [====[
			{
						< 1 Actor1 "<goto Move>"
						>
						< 0 Camera
							Position Camera6
							DepthOfField 50
						>
			]====]

cutsceneFinalFire = [====[
						< 0.3 Actor4 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0 Actor8 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor5 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0.3 Actor7 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0 Actor9 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0 Actor6 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor16
						>
						< 0 Actor15 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor12 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor14 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor11 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor13 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor10 ""
							Action Power:Plasma_Cannon_Shell
							 Target Actor17
						>
						< 0 Actor1 ""
							Action SetHealth(Health(0))
						>
						< 5 Camera
							Position Camera9
							DepthOfField 150
						>
						< 4 FX
							Action GENERIC/Fades/Fade_5Sec.fx
						>
			]====]


cutsceneEnd = [====[
			< 0 Actor1 ""
					Action DestroyMe
			>
			< 0 Actor2 ""
					Action DestroyMe
			>
			< 0 Actor3 ""
					Action DestroyMe
			>
			< 0 Actor4 ""
					Action DestroyMe
			>
			< 0 Actor5 ""
					Action DestroyMe
			>
			< 0 Actor6 ""
					Action DestroyMe
			>
			< 0 Actor7 ""
					Action DestroyMe
			>
			< 0 Actor8 ""
					Action DestroyMe
			>
			< 0 Actor9 ""
					Action DestroyMe
			>
			< 0 Actor10 ""
					Action DestroyMe
			>
			< 0 Actor11 ""
					Action DestroyMe
			>
			< 0 Actor12 ""
					Action DestroyMe
			>
			< 0 Actor13 ""
					Action DestroyMe
			>
			< 0 Actor14 ""
					Action DestroyMe
			>
			< 0 Actor15 ""
					Action DestroyMe
			>
			< 0 Actor16 ""
					Action DestroyMe
			>
			< 0 Actor17 ""
					Action DestroyMe
			>
			< 0 Actor18 ""
					Action DestroyMe
			>
			< 1 Actor19 ""
					Action DestroyMe
			>
			}
			]====]

function tick()
	if Mission.ObjectiveIsSucceeded("ScriptCutsceneStart") then
		cutscene = cutsceneStart .. cutsceneFire .. cutsceneCamera4 .. cutsceneFire .. cutsceneCamera5 .. cutsceneFire .. cutsceneCamera6 .. cutsceneFire.. cutsceneFinalFire .. cutsceneEnd
	end

	Encounter.StartCutSceneFromString(cutscene, Encounter.MyEncounter())
	EndScript()
end
