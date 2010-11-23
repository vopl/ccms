var session = router.cd.global.session;
session.data = router.cdGetInstance('session');
delete router.cd.global.session;

